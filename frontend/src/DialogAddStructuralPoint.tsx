import {useState} from "react";
import {Modal, Form, Button, Col} from "react-bootstrap";
import {DbStructuralPoint} from "./DatabaseInterface";
import axios from 'axios';
import {FontAwesomeIcon} from "@fortawesome/react-fontawesome";
import {faThumbtack} from "@fortawesome/free-solid-svg-icons";

type DialogAddStructuralPointProps = {
    projectId: string;
    show: boolean;
    onHide: () => void;
    onSubmit: () => void;
}

const DialogAddStructuralPoint = (props: DialogAddStructuralPointProps) => {

    const [data, setData] = useState<DbStructuralPoint>({
        id: 0,
        uuid: "",
        name: "",
        coordinate_x: 0,
        coordinate_y: 0,
        coordinate_z: 0,
    });
    const [pinned, setPinned] = useState(false);

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const {name, value} = e.target;
        setData({...data, [name]: value,});
    };

    const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
        event.preventDefault();
        await axios.post(process.env.REACT_APP_BACKEND + '/structuralpoints/' + props.projectId, data)
        props.onSubmit();
        if (pinned === false)
            props.onHide();
    };

    return (
        <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
            <Modal.Header closeButton>
                <Modal.Title id="contained-modal-title-vcenter">Add new structural point</Modal.Title>
            </Modal.Header>
            <Form onSubmit={handleSubmit}>
                <Modal.Body>
                    <Form.Group as={Col} md="4" controlId="validationSptName">
                        <Form.Label>Name</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="name"
                            value={data.name}
                            onChange={handleInputChange}
                        />
                        <Form.Control.Feedback>Please provide a valid name.</Form.Control.Feedback>
                    </Form.Group>
                    <Form.Group as={Col} md="4" controlId="validationSptCoordinateX">
                        <Form.Label>X</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="coordinate_x"
                            placeholder="X coordinate"
                            value={data.coordinate_x}
                            onChange={handleInputChange}
                        />
                        <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                    </Form.Group>
                    <Form.Group as={Col} md="4" controlId="validationSptCoordinateY">
                        <Form.Label>Y</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="coordinate_y"
                            placeholder="Y coordinate in UCS"
                            value={data.coordinate_y}
                            onChange={handleInputChange}
                        />
                        <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                    </Form.Group>
                    <Form.Group as={Col} md="4" controlId="validationSptCoordinateZ">
                        <Form.Label>Z</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="coordinate_z"
                            placeholder="Z coordinate in UCS"
                            value={data.coordinate_z}
                            onChange={handleInputChange}
                        />
                        <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                    </Form.Group>
                </Modal.Body>
                <Modal.Footer>
                    <Button variant="link" onClick={() => { setPinned(!pinned) }} >
                        <FontAwesomeIcon icon={faThumbtack } flip={pinned ? "vertical" : "horizontal" } size="2x" />
                    </Button>
                    <Button type="submit">Submit</Button>
                    <Button onClick={props.onHide}>Close</Button>
                </Modal.Footer>
            </Form>
        </Modal>
    );
}

export default DialogAddStructuralPoint;

