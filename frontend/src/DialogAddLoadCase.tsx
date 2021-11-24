import {useState} from "react";
import {Button, Col, Form, Modal} from "react-bootstrap";
import {ActionType, DbLoadCase, LoadType} from "./DatabaseInterface";
import axios from 'axios';

type DialogAddLoadCaseProps = {
    projectId: string;
    show: boolean;
    onHide: () => void;
}

const DialogAddLoadCase = (props: DialogAddLoadCaseProps) => {

    const [data, setData] = useState<DbLoadCase>({
        id: 0,
        uuid: "",
        name: "",
        description: "",
        actionType: ActionType.PERMANENT,
        type: LoadType.STANDARD
    });

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const {name, value} = e.target;
        setData({...data, [name]: value,});
    };

    const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
        event.preventDefault();
        await axios.post(process.env.REACT_APP_BACKEND + '/loadcases/' + props.projectId, data)
            .catch((e) => {
                //! \todo show error message
                console.log(e)
            })
        window.location.reload();
    };

    return (
        <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
            <Modal.Header closeButton>
                <Modal.Title id="contained-modal-title-vcenter">Add new load case</Modal.Title>
            </Modal.Header>
            <Form onSubmit={handleSubmit}>
                <Modal.Body>
                    <Form.Group as={Col} md="4" controlId="validationLoadCaseName">
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
                    <Form.Group as={Col} md="4" controlId="validationLoadCaseDesc">
                        <Form.Label>Description</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="description"
                            value={data.description}
                            onChange={handleInputChange}
                        />
                    </Form.Group>
                </Modal.Body>
                <Modal.Footer>
                    <Button type="submit">Submit</Button>
                    <Button onClick={props.onHide}>Close</Button>
                </Modal.Footer>
            </Form>
        </Modal>
    );
}

export default DialogAddLoadCase;
