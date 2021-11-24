import {useEffect, useState} from "react";
import {Button, Col, Form, Modal, Spinner} from "react-bootstrap";
import {DbPointSupport, DbStructuralPoint, SupportType} from "./DatabaseInterface";
import axios from "axios";
import {FontAwesomeIcon} from "@fortawesome/react-fontawesome";
import {faThumbtack} from "@fortawesome/free-solid-svg-icons";

type DialogAddPointSupportProps = {
    projectId: string;
    show: boolean;
    onHide: () => void;
    onSubmit: () => void;
}

const DialogAddPointSupport = (props: DialogAddPointSupportProps) => {

    const [data, setData] = useState<DbPointSupport>({
        id: 0,
        uuid: "",
        name: "",
        type: SupportType.FIXED,
        referencePoint: {
            id: 0,
            name: "",
            uuid: "",
            coordinate_x: 0,
            coordinate_y: 0,
            coordinate_z: 0,
        }
    });
    const [structurePointList, setStructurePointList] = useState<DbStructuralPoint[]>([]);
    const [loading, setLoading] = useState(true)
    const [selectedIndex, setSelectedIndex] = useState(0);
    const [pinned, setPinned] = useState(false);

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const {name, value} = e.target;
        setData({...data, [name]: value,});
    };

    const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
        event.preventDefault();
        await axios.post(process.env.REACT_APP_BACKEND + '/pointsupport/' + structurePointList[selectedIndex].id, data)
        props.onSubmit();
        if (pinned === false)
            props.onHide();
    };

    useEffect(() => {
        axios.get(process.env.REACT_APP_BACKEND + '/structuralpoints/' + props.projectId)
            .then(response => setStructurePointList(response.data))
            .finally(() => setLoading(false))
    }, [props.show, props.projectId]);

    return (
        <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
            <Modal.Header closeButton>
                <Modal.Title id="contained-modal-title-vcenter">Add new point support</Modal.Title>
            </Modal.Header>
            <Form onSubmit={handleSubmit}>
                <Modal.Body>
                    <Form.Group as={Col} md="4" controlId="validationPointSupportName">
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
                    {loading && <Spinner animation="border" role="status"></Spinner>}
                    <Form.Group controlId="ControlSelectBegin">
                        <Form.Label>attached</Form.Label>
                        <Form.Control as="select" onChange={(e: any) => { setSelectedIndex(e.target.selectedIndex); }}>
                            {structurePointList.map(e => (<option key={e.id}>{e.name}</option>))}
                        </Form.Control>
                    </Form.Group>
                </Modal.Body>
                <Modal.Footer>
                    <Button variant="link" onClick={() => {
                        setPinned(!pinned)
                    }}>
                        <FontAwesomeIcon icon={faThumbtack} flip={pinned ? "vertical" : "horizontal"} size="2x"/>
                    </Button>
                    <Button type="submit">Submit</Button>
                    <Button onClick={props.onHide}>Close</Button>
                </Modal.Footer>
            </Form>
        </Modal>
    );
}

export default DialogAddPointSupport;
