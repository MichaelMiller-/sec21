import {useState} from "react";
import {Button, Modal, Form, Col} from "react-bootstrap";
import {DbMaterial} from "./DatabaseInterface";
import axios from 'axios';
import {FontAwesomeIcon} from "@fortawesome/react-fontawesome";
import {faThumbtack} from "@fortawesome/free-solid-svg-icons";

type DialogAddMaterialProps = {
    projectId: string;
    show: boolean;
    onHide: () => void;
    onSubmit: () => void;
}

const DialogAddMaterial = (props: DialogAddMaterialProps) => {

    const [data, setData] = useState<DbMaterial>({
        id: 0,
        uuid: "",
        name: "",
        E_modulus: 0
    })
    const [pinned, setPinned] = useState(false);

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const {name, value} = e.target;
        setData({...data, [name]: value,});
    };

    const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
        event.preventDefault();
        await axios.post(process.env.REACT_APP_BACKEND + '/materials/' + props.projectId, data)
        props.onSubmit();
        if (pinned === false)
            props.onHide();
    };

    return (
        <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
            <Modal.Header closeButton>
                <Modal.Title id="contained-modal-title-vcenter">Add new material</Modal.Title>
            </Modal.Header>
            <Form onSubmit={handleSubmit}>
                <Modal.Body>
                    <Form.Group as={Col} md="4" controlId="validationCustom01">
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
                    <Form.Group as={Col} md="4" controlId="validationCustom02">
                        <Form.Label>E modules [MPa]</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="E_modulus"
                            value={data.E_modulus}
                            onChange={handleInputChange}
                        />
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

export default DialogAddMaterial;